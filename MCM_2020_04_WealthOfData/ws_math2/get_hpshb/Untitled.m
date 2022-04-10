pacifier_parent = pacifier_all(:,5);
pacifier_star = pacifier_all(:,8);
%pacifier_headline = pacifier_all(:,13);
%pacifier_body = pacifier_all(:,14);

pacifier_headline = get_word(pacifier_all,13);
pacifier_body = get_word(pacifier_all,14);
pacifier_headline = pacifier_headline(:,2:end);
pacifier_body = pacifier_body(:, 2:end);
